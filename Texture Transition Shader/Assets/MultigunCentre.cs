using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MultigunCentre : MonoBehaviour {

    public float rotateSpeed;

    public GameObject unlitShip;
    public GameObject shadedShip;

	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        Rotate();
	}

    void Rotate()
    {
        transform.Rotate(0, rotateSpeed * Time.deltaTime, 0);
    }

    public void EnableUnlit()
    {
        shadedShip.SetActive(false);
        unlitShip.SetActive(true);
    }

    public void EnableShaded()
    {
        shadedShip.SetActive(true);
        unlitShip.SetActive(false);
    }
}
