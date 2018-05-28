using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MultigunCentre : MonoBehaviour {

    public float rotateSpeed;

    public GameObject[] ships;

	void Start ()
    {
        EnableShip(4);
	}

	void Update ()
    {
        Rotate();
	}

    void Rotate()
    {
        transform.Rotate(0, 0, rotateSpeed * Time.deltaTime);
    }

    public void EnableShip(int shipID)
    {
        foreach (var ship in ships)
        {
            ship.GetComponent<MeshRenderer>().enabled = false;
        }

        ships[shipID].GetComponent<MeshRenderer>().enabled = true;
    }
}
