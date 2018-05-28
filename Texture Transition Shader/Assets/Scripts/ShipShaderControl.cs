using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ShipShaderControl : MonoBehaviour {

    public float TransitionSpeed;
    public float ShipDisolveTarget;
    public float ShipDisolveAmount;

    public Image previewTransitionImage;
    public GameObject previewRenderer;

    public Sprite[] sprites;

    bool usingClouds = false;
    public Sprite[] cloudSprites;

    // Use this for initialization
    void Start () {
        SwitchTransition(0);
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (this.gameObject.activeSelf == false) { return; }

        ShipDisolveAmount = Mathf.MoveTowards(ShipDisolveAmount, ShipDisolveTarget, TransitionSpeed);

        if (Input.GetKeyDown(KeyCode.Space))
        {
            Transition();
        }

        this.GetComponent<Renderer>().material.SetFloat("DissolveAmount", ShipDisolveAmount);
        previewRenderer.GetComponent<Image>().material.SetFloat("DissolveAmount", ShipDisolveAmount);
    }

    public void Transition()
    {
        if(this.gameObject.activeSelf == false){ return; }

        if (ShipDisolveAmount <= 0 || ShipDisolveAmount >= 1)
        {
            if (usingClouds)
            {
                int id = Random.Range(0, cloudSprites.Length - 1);
                this.GetComponent<Renderer>().material.SetTexture("TransitionTexture", cloudSprites[id].texture);
                previewRenderer.GetComponent<Image>().material.SetTexture("TransitionTexture", cloudSprites[id].texture);
                previewTransitionImage.sprite = cloudSprites[id];
            }

            if (ShipDisolveTarget == 1)
            {
                ShipDisolveTarget = 0;
            }
            else
                if (ShipDisolveTarget == 0)
            {
                ShipDisolveTarget = 1;
            }
        }
    }

    public void SwitchTransition(int id)
    {
        if(id == 0)
        {
            usingClouds = true;
        }

        this.GetComponent<Renderer>().material.SetTexture("TransitionTexture", sprites[id].texture);
        previewRenderer.GetComponent<Image>().material.SetTexture("TransitionTexture", sprites[id].texture);
        previewTransitionImage.sprite = sprites[id];
    }
}
